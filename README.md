# cema internship
software engineering submission for [cema internship](https://cema-africa.uonbi.ac.ke/index.php/news-and-media/opportunities/cema-internship-opportunities). received on 24th April 2025, 8:23am, must be submitted by 27th April 2025.

# design

## backend
[django](https://www.djangoproject.com/), building fast and reliable api's . also a decent backend server

## frontend
[qt](https://www.qt.io), crossplatform toolkit for high performance native graphical interfaces 

## ci 
github actions ci and release hosts a build of the qt front end for windows and linux

# clone 
```
# you can skip the --recursive, it's only if building on/for msys2/mingw
git clone --recursive --depth 1 https://github.com/Denzy7/cema-internship
cd cema-internship 

# you can also skip submodules if not building for/on msys2/mingw
git submodule update --init --recursive --depth 1
```

# building the qt frontend
cmake and qt5 or qt6.
```bash
cmake -S frontend_qt/patientmgr_qt -B build
cmake --build build
cmake --install build --prefix out
```

# running
```bash
# backend
cd backend_django
python3 -m venv venv

# from here if on windows, use ./venv/Scripts/pip or ./venv/Scripts/python
# alternatively you could activate venv, but remember to deactivate
# ./venv/Scripts/activate || /venv/Scripts/python
#
# we'll use an alias for now for (linux)

alias vpip=./venv/bin/pip
alias vpy=./venv/bin/python

vpip install -r requirements.txt 

# only do migration the first time
vpy manage.py makemigrations patientmgr
vpy manage.py migrate

# create superuser for the first time only
vpy manage.py createsuperuser

# run!
vpy manage.py runserver 8000

# frontend, in a seprate shell session
./out/patientmgr_qt

##########################################
#                                        #
# now you just connect  on the front end #
# or you can also use the django admin   #
#                                        #
##########################################
```
