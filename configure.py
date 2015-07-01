#!/bin/env python

import subprocess

def test_irrlicht_compile_flags(cflags, ldflags):
    try:
        with open("/tmp/irr_test.cpp", "w") as fout:
            fout.write(r"""
#include <irrlicht.h>
using namespace irr;
int main() {
  IrrlichtDevice *device =
            createDevice(video::EDT_NULL, core::dimension2d<u32>(800, 600), 32, true, false, true, NULL);
}
""")
        exit_code = subprocess.call("g++ /tmp/irr_test.cpp -o /tmp/irr_test "+cflags+" "+ldflags, shell=True)
        print "Exit code",exit_code
        return exit_code==0;
    except:
        print " -- [EXCEPTION] --"
        print "- write /tmp/test.cpp"
        print "- g++ /tmp/test.cpp -o /tmp/test "+cflags+" "+ldflags
        print "One of these operation caused an exception..."
        raise
    finally:
        try:
            os.unlink("/tmp/irr_test.cpp")
            os.unlink("/tmp/irr_test")
        except:
            # at worst we leave things over the place
            pass


def find_irrlicht_compile_flags():
    cflags="-I./irrlicht-1.8.1/include -I/usr/X11R6/include"
    ldflags="-L./irrlicht-1.8.1/lib/Linux -lIrrlicht"
    ldflags+=" -L/usr/X11R6/lib64 -lGL -lXxf86vm -lXext -lX11 -lXcursor"
    if test_irrlicht_compile_flags(cflags,ldflags):
        return (cflags, ldflags)

    cflags="-I/usr/X11R6/include -I/usr/include/irrlicht/"
    ldflags="-lIrrlicht -L/usr/X11R6/lib64 -lGL -lXxf86vm -lXext -lX11 -lXcursor"
    if test_irrlicht_compile_flags(cflags, ldflags):
        return (cflags, ldflags)
    return (None, None)







if __name__=="__main__":
    cflags, ldflags = find_irrlicht_compile_flags()
    if cflags == None or ldflags==None:
        print "Error: Cannot find a a working irr config."
        exit(1)
    print "Config:"
    print "LDFLAGS="+ldflags
    print "CPPFLAGS="+cflags
    with open("makefile.config","w") as f:
        f.write("LDFLAGS="+ldflags+"\n")
        f.write("CPPFLAGS="+cflags+"\n")

    print "Configure successful"
