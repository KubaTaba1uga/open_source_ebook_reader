import os
import sys

# -- Project information -------------------------------------------------------
project = 'Open Source eBook Reader'
author = 'Jakub Buczynski <KubaTaba1uga>'

# -- General configuration -----------------------------------------------------
extensions = [
    'breathe',
    "sphinx.ext.autodoc",
    "sphinx.ext.extlinks",
    "sphinx.ext.intersphinx",
    "sphinx.ext.mathjax",
    "sphinx.ext.todo",
    "sphinx.ext.viewcode",
    "myst_parser",    
]

breathe_projects = {
    project: os.path.abspath(os.path.join('..', 'build', 'doxygen', 'xml')),
}
breathe_default_project = project

templates_path = ['_templates']
exclude_patterns = []

# -- Options for HTML output ---------------------------------------------------
html_theme = "sphinx_rtd_theme"


html_theme_options = {
}

html_context = {
    "display_github": True,
    "github_user": "KubaTaba1uga",
    "github_repo": "open_source_ebook_reader",
    "github_version": "main",
    "conf_py_path": "/docs/",
}

html_short_title = "eBook Reader"
pygments_style = "monokai"
pygments_dark_style = "monokai"

copyright = "2026, Jakub Buczynski <KubaTaba1uga>"
