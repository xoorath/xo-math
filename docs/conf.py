import subprocess, os
import sys

sys.path.append( "./" )

on_rtd = os.environ.get('READTHEDOCS', None) == 'True'
if on_rtd:
  from subprocess import call 
  call('doxygen')

needs_sphinx = '1.4.3'

extensions = ['breathe', 'sphinx.ext.imgmath']

breathe_projects = {
    "xo-math":"./xml/",
}
breathe_default_members = ('members', 'undoc-members')

imgmath_image_format = 'svg'
imgmath_font_size = 16

templates_path = ['_templates']

source_suffix = '.rst'
master_doc = 'index'

project = u'xo-math'
copyright = u'2016, Jared Thomson'
author = u'Jared Thomson'
version = u'1.2b0'
release = u'1.2b0'

language = None

exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

pygments_style = 'sphinx'

todo_include_todos = False

html_theme = "sphinx_rtd_theme"

html_theme_path = ["_themes"]

html_static_path = ['_static']

htmlhelp_basename = 'xo-mathdoc'

latex_elements = {
}

latex_documents = [
    (master_doc, 'xo-math.tex', u'xo-math Documentation',
     u'Jared Thomson', 'manual'),
]

man_pages = [
    (master_doc, 'xo-math', u'xo-math Documentation',
     [author], 1)
]

texinfo_documents = [
    (master_doc, 'xo-math', u'xo-math Documentation',
     author, 'xo-math', 'One line description of project.',
     'Miscellaneous'),
]