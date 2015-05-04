#!/usr/bin/env python
# -*- coding: UTF-8 -*-
# vim: set fileencoding=utf-8

'''
 By Ivanprjcts
'''

import unittest

from lib.testutil import exec_cmd



class TestXRainbowCrack(unittest.TestCase):

    @classmethod
    def setUpClass(cls):
        pass

    @classmethod
    def tearDownClass(cls):
        pass


    def test_help(self):
        out, err = exec_cmd("xrainbow_crack -h")
        self.assertTrue("Usage: " in out)

    def test_no_args(self):
        out, err = exec_cmd("xrainbow_crack")
        self.assertTrue("Usage: " in out)


if __name__ == '__main__':
    unittest.main()
