#!/bin/bash

pip install virtualenv
virtualenv venv
source venv/bin/activate
pip3 install -r server/requirements.txt
python3 server/manage.py runserver
