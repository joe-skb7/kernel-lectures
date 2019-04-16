# Kernel lectures

These are my Linux kernel lectures for GlobalLogic kernel course. Sources are
in LaTeX format. Compiled PDF files are included as well.

In `bbb-tutorial/` directory you can find my up-to-date tutorial for BeagleBone
Black bring up.

## Build dependencies

Next packages have to be installed before building kernel lectures:

- fonts-fira (FiraSans fonts)
- python-pygments
- rubber
- texlive-common
- texlive-fonts-extra
- texlive-fonts-recommended
- texlive-latex-extra
- texlive-latex-recommended
- texlive-luatex
- texlive-xetex

For `fonts-fira` you can build and install deb-package from
[https://github.com/joe-skb7/pkg-fonts-fira](here):

```
$ sudo dpkg -i fonts-fira*.deb
```

Other packages can be installed from Debian/Ubuntu repo:

```
$ sudo apt update
$ sudo apt install python-pygments rubber texlive-common texlive-fonts-extra \
                   texlive-fonts-recommended texlive-latex-extra \
                   texlive-latex-recommended texlive-luatex texlive-xetex
```

## Build instructions

Just issue `make` command to build all lectures:

```
$ make
```

Or do the same in particular sub-directory.

## Authors

**Sam Protsenko**

## License

This project is licensed under the GPLv3.

## References

[1] http://tug.ctan.org/macros/latex2e/contrib/beamer/doc/beameruserguide.pdf

[2] https://rosschurchley.com/2016/03/13/metropolis-beamer-theme/

[3] https://rosschurchley.com/2016/03/14/owl-beamer-color-theme/

[4] http://mirrors.ibiblio.org/CTAN/macros/latex/contrib/beamer-contrib/themes/metropolis/doc/metropolistheme.pdf

[5] http://ctan.math.washington.edu/tex-archive/macros/latex/contrib/beamer-contrib/themes/beamercolorthemeowl/beamercolorthemeowl.pdf

[6] https://github.com/matze/mtheme/blob/master/demo/demo.tex
