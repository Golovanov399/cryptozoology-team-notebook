#!/usr/bin/python3

import os
import codecs

header = """\\documentclass[10pt,twocolumn]{article}
\\usepackage[T1]{fontenc}
\\usepackage[english,russian]{babel}
\\usepackage[utf8x]{inputenc}
\\usepackage{amsthm, amssymb}
\\usepackage{amsmath}
\\usepackage{lmodern}
\\usepackage{graphicx}

\\usepackage{fontspec}
\\defaultfontfeatures{Mapping=tex-text}
\\setmainfont{CMU Serif}
\\setsansfont{CMU Sans Serif}
\\setmonofont{CMU Typewriter Text}

\\usepackage{minted}

\\pagestyle{myheadings}

\\textheight = 780pt
\\textwidth = 530pt
\\voffset = -3cm
\\hoffset = -1.5cm

\\renewcommand{\\baselinestretch}{0.85}

\\makeatletter
\\renewcommand{\\@oddhead}{\\tt Moscow I of Physics and Technology
\\hfil\\thepage}
\\makeatother

\\begin{document}

\\begin{titlepage}
\\includegraphics[height=.9\\textheight]{titlepic}
\\end{titlepage}

\\small
\\usemintedstyle{pastie}
\\tableofcontents
"""
footer = """\\end{document}
"""
bookname = "book.tex"
minted_begin = "\\begin{minted}[mathescape,breaklines,tabsize=4,breaksymbolright=\\tiny\\ensuremath{\\hookleftarrow}]"
minted_end = "\\end{minted}"


allowed_extensions = ['.cpp', '.h', '.py', '.txt', '.tex']
ext_to_lang = {'.cpp': 'c++', '.h': 'c++', '.py': 'python'}
excluded_files = ['make_book.py', 'fft_test_double.cpp', 'fft_test_integer.cpp', 'fft_advanced_double.h', 'extended_euclidean.h']


def get_minted(text, lang='c++'):
	if lang == "" or lang is None:
		lang = 'text'
	return "%s{%s}\n%s\n%s\n" % (minted_begin, lang, text, minted_end)


book_text = ""


def escaped(s):
	return s.replace('_', '\\_')


def add_file(path, num_subs=0):
	ext = os.path.splitext(path)[-1]
	if ext not in allowed_extensions:
		return
	if os.path.basename(path) in excluded_files:
		return
	global book_text
	book_text += "\\" + "sub" * num_subs + "section{\\tt %s}\n" % escaped(os.path.basename(path))
	if ext == '.tex':
		book_text += "\\input{%s}\n" % path
	else:
		with codecs.open(path, 'r', 'utf-8') as f:
			book_text += get_minted(f.read(), ext_to_lang.get(ext))


def fill_book(path, depth=-1):
	global book_text
	if os.path.basename(path)[0] == '.' and path != '.':
		return
	if os.path.basename(path)[0] == '_':
		return
	if depth >= 0:
		book_text += "\\" + "sub" * depth + "section{\\tt %s}\n" % escaped(os.path.basename(path))
	depth += 1
	opts = list(map(lambda x: path + '/' + x, sorted(os.listdir(path))))
	dirs = list(filter(os.path.isdir, opts))
	files = list(filter(os.path.isfile, opts))
	for d in dirs:
		fill_book(d, depth)
	for f in files:
		add_file(f, depth)


os.system("rm -f %s" % bookname)


book_text += header
fill_book('.')
book_text += footer


with codecs.open(bookname, "w", "utf-8") as f:
	print(book_text, file=f)

os.system("xelatex -shell-escape -8bit %s" % bookname)
