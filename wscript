#! /usr/bin/env python
# encoding: utf-8

'''
@author: Milos Subotic <milos.subotic.sm@gmail.com>
@license: MIT

@brief: Waf script just for distclean and dist commands.
'''

###############################################################################

import os
import fnmatch
import shutil
import datetime

import waflib

###############################################################################

APPNAME = 'MWT_Source'

top = '.'

###############################################################################

def recursive_glob(pattern, directory = '.'):
	if pattern.startswith('/'):
		for root, dirs, files in os.walk(directory, followlinks = True):
			if root == directory:
				for f in files:
					if fnmatch.fnmatch('/' + f, pattern):
						yield os.path.join(root, f)
				for d in dirs:
					if fnmatch.fnmatch('/' + d + '/', pattern):
						yield os.path.join(root, d)
	else:
		for root, dirs, files in os.walk(directory, followlinks = True):
			for f in files:
				if fnmatch.fnmatch(f, pattern):
					yield os.path.join(root, f)
			for d in dirs:
				if fnmatch.fnmatch(d + '/', pattern):
					yield os.path.join(root, d)

def collect_git_ignored_files():
	for gitignore in recursive_glob('.gitignore'):
		with open(gitignore) as f:
			base = os.path.dirname(gitignore)

			for pattern in f.readlines():
				pattern = pattern.rstrip() # Remove new line stuff on line end.
				for f in recursive_glob(pattern, base):
					yield f

###############################################################################

def distclean(ctx):
	for fn in collect_git_ignored_files():
		if fn not in ['./download', './out']:
			if os.path.isdir(fn):
				shutil.rmtree(fn)
			else:
				os.remove(fn)

def dist(ctx):
	now = datetime.datetime.now()
	time_stamp = '{:d}-{:02d}-{:02d}-{:02d}-{:02d}-{:02d}'.format(
		now.year,
		now.month,
		now.day,
		now.hour,
		now.minute,
		now.second
	)
	ctx.arch_name = '../{}-{}.zip'.format(APPNAME, time_stamp)
	ctx.algo = 'zip'
	ctx.base_name = APPNAME
	# Also pack git.
	waflib.Node.exclude_regs = waflib.Node.exclude_regs.replace(
'''
**/.git
**/.git/**
**/.gitignore
**/download
**/download/**
**/out
**/out/**''', '')
	# Ignore waf's stuff.
	waflib.Node.exclude_regs += '\n**/.waf*'
	
###############################################################################

