import glob
import serial
import time
import datetime
import wx
from wx.lib.analogclock import *
import struct

MAX_ALARM_DID = 0x3F

class PingError(Exception):
    pass

class MsgDef:
    def __init__(self, v, const):
        v = v.strip()
        v = v[1:-1]
        val, msg_size, callback = v.split(',')
        msg_size = msg_size.strip()
        if 'x' in val:
            self.val = int(val, 16)
        else:
            if val not in const:
                print const.keys()
                raise Exception('Unknown constant %s' % val)
            self.val = const[val]
        try:
            self.n_byte = int(msg_size)
        except:
            try:
                self.n_byte = const[msg_size]
            except:
                # print msg_size in const.keys()
                # print const.keys()
                raise
        self.callback = callback
    def __str__(self):
        return chr(self.val)
        
def read_constants(fn):
    f = open(fn)
    out = {}
    for line in f.readlines():
        if line.startswith('const'):
            line = line.split(';')[0]
            try:
                d, v = line.split('=')
                c, t, n = d.split()
                if 'int' in t:
                    if 'X' in v.upper():
                        base = 16
                    else:
                        base = 10
                        out[n] = int(v, base)
                elif t == 'MsgDef':
                    try:
                        out[n] = MsgDef(v, out)
                    except Exception, e:
                        if '*' not in line:
                            print 'problem with ', line, e
                else:
                    out[n] = v
            except:
                pass
                # print 'could not read', line
    return out
            
class Struct:
    def __init__(self, **dict):
        self.d = dict
    def __getattr__(self, name):
        return self.d[name]
    def __add__(self, other):
        out = {}
        for key in self.d:
            out[key] = self.d[key]
        for key in other.d:
            out[key] = other.d[key]
        return Struct(**out)

const = {}
c_files = ['ClockTHREE_02.pde']
c_files.extend(glob.glob("../../*.cpp"))
c_files.extend(glob.glob("../../*.h"))
for file in c_files:
    next = read_constants(file)
    for key in next:
        const[key] = next[key]
const = Struct(**const)

serialport = '/dev/ttyUSB0'

# raw_input('...')
ser = serial.Serial(serialport, baudrate=const.BAUDRATE, timeout=.5)
ser.flush()

payload = 'A23456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789'
gmt_offset = -5 * 3600

def time_req():
    ser.flush()
    ser.write(str(const.ABS_TIME_REQ))
    dat = ser.read(4)
    if len(dat) < 4:
        out = 0
    else:
        out = struct.unpack('<I', dat)[0]
    return out

def time_set():
    now = int(round(time.time())) + gmt_offset
    dat = struct.pack('<I', now)
    ser.write(str(const.ABS_TIME_SET))
    ser.write(dat)
    
def set_tod_alarm(h, m, s, is_set):
    dat = (h * 60 + m) * 60 + s
    dat = struct.pack('<I', dat)
    ser.write(str(const.TOD_ALARM_SET))
    ser.write(dat)
    ser.write(chr(is_set))

def get_tod_alarm():
    ser.write(str(const.TOD_ALARM_REQ))
    ser_data = ser.read(6)
    if len(ser_data) < 6:
        raise ValueError('Got bad tod_alarm_msg, l=%d: "%s"' % (len(ser_data), ser_data))
    assert ser_data[0] == str(const.TOD_ALARM_SET)
    hms = struct.unpack('<I', ser_data[1:5])[0]
    h, ms = divmod(hms , 60 * 60)
    m, s = divmod(ms, 60)
    return h, m, s, bool(ser_data[5])

def set_data(id, data):
    # MID, len(payload)
    l = len(data)
    out = '%s%s%s%s%s' % (str(const.DATA_SET), chr(l + 4), id, chr(l + 2),  data)
    assert len(out) == len(data) + 4, '%s != %s' % (len(out), len(data) + 4)
    ser.write(out)
    time.sleep(.1)
    err = ser.read(4)
    if err:
        raise Exception(get_err(err))

def get_err(err):
    id = '0x%02x' % ord(err[0])
    len = ord(err[1])
    # print id, len, err[2:len]
    return err[2:]

def err_check():
    err = ser.read(1024)
    if err:
        print err
    assert len(err) == 0, get_err(err)

class EEPROM:
    # mirror of C3 EEPROM
    def __init__(self):
        self.read()

    def read(self):
        ser.write(str(const.EEPROM_DUMP))
        self.eeprom = ser.read(const.MAX_EEPROM_ADDR + 1)
        if len(self.eeprom) < const.MAX_EEPROM_ADDR + 1:
            raise ValueError("Could not read EEPROM")

    def get_dids(self):
        n = ord(self.eeprom[-1])
        addr = 0
        out = {}
        for i in range(n):
            did = self.eeprom[addr]
            l = ord(self.eeprom[addr + 1])
            out[did] = (addr, self.eeprom[addr + 2: addr + l])
            addr += l
        return out

    def next_did(self, alarm_flag=False):
        dids = self.get_dids().keys()
        if alarm_flag:
            out = chr(1)
        else:
            out = chr(0x3F)
        while out in dids and ord(out) < 255:
            out = chr(ord(out) + 1)
        return out
    
    def add_record(self, record, alarm_flag=False):
        did = None
        if record in self.eeprom:
            addr = self.eeprom.find(record) - 2
            if ord(self.eeprom[addr + 1]) == len(record) + 2:
                # we already have it
                did = self.eeprom[addr]
        if did is None:
            did = self.next_did(alarm_flag)
            if did < MAX_ALARM_DID:
                assert len(record) + 2 == ALARM_RECORD_LEN
            self.write(did, record)
        return did

    def write(self, did, record):
        set_data(did, record)
        self.read()
        
def eeprom_read():
    err_check()
    ser.write(str(const.EEPROM_DUMP))
    eeprom = ser.read(1024)
    if len(eeprom) -- 1024:
        n = ord(eeprom[-1])
    else:
        raise ValueError('Eeprom is not 1024 bytes!')
    for r in range(64):
        print '%04d 0x%03x  -- ' % (r * 16, r * 16),
        for i in range(16):
            print '%02x' % ord(eeprom[r * 16 + i]),
        print ""
    print ""
    print 'N:', n
    addr = 0
    try:
        for i in range(n):
            did = eeprom[addr]
            l = ord(eeprom[addr + 1])
            print ord(did), l, 
            for j in range(2, l):
                print '0x%02x' % ord(eeprom[addr + j]),
            print eeprom[addr + 2: addr + l]
            addr += l
    except:
        pass

class ClockTHREE_Error(Exception):
    pass
def get_data(id):
    ser.write(str(const.DATA_REQ))
    ser.write(id)
    head = ser.read(1)
    if head == str(const.ERR_OUT):
        l = ord(ser.read(1))
        err = 'ERROR: ' + ser.read(l)
        raise ClockTHREE_Error(err)
    assert head == str(const.DATA_SET), ('? 0x%x' % ord(head))
    n_byte = ord(ser.read(1))
    # print 'N_BYTE:', n_byte
    assert ser.read(1) == id
    
    out = ser.read(n_byte - 3)
    # print 'out:', out
    # print 'equal?', len(out), n_byte - 3
    assert ord(out[0]) - 1 == len(out), '%s != %s' % (ord(out[0]), len(out))
    return out[1:]

def sync():
    ser.write(str(const.SYNC))

def ping():
    data_out = str(const.PING) + payload[:const.PING.n_byte - 1]
    assert len(data_out) == const.PING.n_byte, (
        'len(out) != %s' % const.PING.n_byte)
    ser.write(data_out)
    out = ser.read(const.PING.n_byte - 1)
    # print out
    if out != data_out[1:]:
        raise PingError('Recieved bad ping data:"%s"' % out)
    return True

def clear_eeprom():
    # print '0x%02x' % const.EEPROM_CLEAR.val
    out = str(const.EEPROM_CLEAR) * const.EEPROM_CLEAR.n_byte
    assert len(out) == const.EEPROM_CLEAR.n_byte
    ser.write(out)
    time.sleep(5)

def delete_did(did):
    ser.write(str(const.DATA_DELETE))
    ser.write(did)
    time.sleep(1)
def trigger_mode():
    ser.write(str(const.TRIGGER_MODE) * const.TRIGGER_MODE.n_byte)

def scroll_data(did):
    ser.write(str(const.SCROLL_DATA))
    ser.write(did)

def set_alarm(t, countdown, repeat, scroll_msg, 
              effect_id, sound_id):
    eeprom = EEPROM()
    scroll_did = eeprom.add_record(scroll_msg)
    dat = [
           struct.pack('<I', t),
           chr(countdown),
           chr(repeat),
           scroll_did,
           chr(effect_id),
           chr(sound_id)
           ]
    msg = ''.join(dat)
    assert len(msg) == 9

    out = eeprom.add_record(msg, alarm_flag=True)
    
    # finally inform C3 new alarm is waiting
    ser.write(str(const.ANNIVERSARY))
    ser.write(out)
    return out

def main():
    # clear_eeprom()
    # eeprom_read()
    set_alarm(time_req() + 5, 0, 0, "DONE!", 0, 0) ## HERE IS WHY!!  GMT offset of such?
    eeprom_read()
    trigger_mode()
    return
    eeprom_read()
    print 'PING ok?'
    for i in range(3):
        if ping():
            print '    YES!'
    clear_eeprom()
    ping()

    now = time.gmtime(time_req())
    year = now.tm_year
    print year
    if year != 2011:
        raise 'restart, got bad year'

    for i in range(3):
        now = time.gmtime(time_req())
        assert (now.tm_year == 2011), 'huh, year=%s' % now.tm_year
        print i, year
    set_tod_alarm(11, 12, 13, True)
    print get_tod_alarm()
    
    ping()
    print 'ping ok'

    time_set()
    print time_req()

    h = 0
    m = 0
    s = 0
    is_set = True
    set_tod_alarm(h, m, s, is_set)
    ahh, amm, ass, ais_set = get_tod_alarm()
    print ahh, amm, ass, h, m, s
    assert ahh == h and amm == m and ass == s

    for i in range(1):
        t = time_req()
        print time.gmtime(t)
        # print time.gmtime(t).tm_sec, time.gmtime(time.time()).tm_sec
        time.sleep(.1)
    
    clear_eeprom()
    J = 'J'
    set_data(J, 'JS: TEST')
    delete_did(J)
    err_check()

    eeprom_read()
    clear_eeprom()
    print 'eeprom cleared?'
    eeprom_read()

    err_check()
    eeprom_read()
    err_check()
    msg = 'J--2'
    set_data(J, msg)
    eeprom_read()
    assert get_data(J) == msg
    err_check()

    msg = 'This is a test. '
    set_data(chr(12), msg + '12')
    err_check()
    print 'msg 12?', get_data(chr(12))
    delete_did(chr(12))
    set_data(chr(1), msg + '1')
    err_check()
    print get_data(chr(1))
    set_data(chr(2), msg + '2')
    err_check()
    print get_data(chr(2))
    eeprom_read()
    clear_eeprom()
    eeprom_read()
if __name__ == '__main__':
    main()
