#!/bin/bash

pip install virtualenv
virtualenv venv
source venv/bin/activate
python3 init.py
python3 server/manage.py runserver
