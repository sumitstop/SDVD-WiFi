
#!/usr/bin/python2

import sys, os, re
sys.path.append('')
os.environ['srcdir'] = '/home/john/myprefix/src/gr-ieee-80211/lib'
os.chdir('/home/john/myprefix/src/gr-ieee-80211/lib/lib')

if __name__ == '__main__':
    sys.path.append('/home/john/myprefix/src/gr-ieee-80211/lib/../python')
    import build_utils
    root, inp = sys.argv[1:3]
    for sig in sys.argv[3:]:
        name = re.sub ('X+', sig, root)
        d = build_utils.standard_dict(name, sig, 'ieee802_11')
        build_utils.expand_template(d, inp, '_impl')
