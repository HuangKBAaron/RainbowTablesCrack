#!/usr/bin/env python
# -*- coding: UTF-8 -*-
# vim: set fileencoding=utf-8

'''
 By Ivanprjcts
'''

import unittest
import shutil
import os

from os.path import expanduser
from lib.testutil import exec_cmd




class TestXRainbowCrack(unittest.TestCase):

    APP_NAME = 'xrainbow_crack'

    @classmethod
    def setUpClass(cls):
        TestXRainbowCrack.home = expanduser("~")

        if os.path.isdir(TestXRainbowCrack.home + '/xRainbowCrack/rbt_test'):
            shutil.rmtree(TestXRainbowCrack.home + '/xRainbowCrack/rbt_test')

    @classmethod
    def tearDownClass(cls):
        shutil.rmtree(TestXRainbowCrack.home + '/xRainbowCrack/rbt_test')


    def test_help(self):
        out, err = exec_cmd(self.APP_NAME + " -h")

        self.assertTrue("Usage: " in out)

    def test_no_args(self):
        out, err = exec_cmd(self.APP_NAME)

        self.assertTrue("Usage: " in out)

    def test0_generate_args(self):
        out, err = exec_cmd(self.APP_NAME + " -g -m 5 -s u -c 7 -l 8 -n 9")

        # point0
        self.assertTrue("gflag_0: 1" in out, out)
        self.assertTrue("maxlen_0: 5" in out)
        self.assertTrue("charset_0: 4" in out)
        self.assertTrue("chainlen_0: 7" in out)
        self.assertTrue("tablelen_0: 8" in out)
        self.assertTrue("ntables_0: 9" in out)

        # point1
        self.assertTrue("maxlen_1: 5" in out)
        self.assertTrue("charset_1: 4" in out)
        self.assertTrue("chainlen_1: 7" in out)
        self.assertTrue("tablelen_1: 8" in out)
        self.assertTrue("ntables_1: 9" in out)
        self.assertTrue("nthreads_1: 8" in out)
        self.assertTrue("package_1: " + self.home + "/xRainbowCrack/rbt_test" in out)

    def test1_analyze_args(self):
        out, err = exec_cmd(self.APP_NAME + " -a " + self.home + "/xRainbowCrack/rbt_test")

        # point3
        self.assertTrue("avalue_2: " + self.home + "/xRainbowCrack/rbt_test" in out)
        self.assertTrue("maxlen_2: 5" in out)
        self.assertTrue("charset_2: 4" in out)
        self.assertTrue("chainlen_2: 7" in out)
        self.assertTrue("ntables_2: 9" in out)

    def test2_crack_args(self):
        out, err = exec_cmd(self.APP_NAME + " -r resources/sample_hashed_passwords.txt -f " + self.home + "/xRainbowCrack/rbt_test")

        # point2
        self.assertTrue("rvalue_3: resources/sample_hashed_passwords.txt" in out)
        self.assertTrue("fvalue_3: "+ self.home + "/xRainbowCrack/rbt_test" in out)



if __name__ == '__main__':
    unittest.main()
