# Generated by Django 4.1.7 on 2023-04-29 18:32

from basic.models import User
from django.conf import settings
from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('basic', '0012_alter_chessgame_black_alter_chessgame_result_and_more'),
    ]

    operations = [
        migrations.AlterField(
            model_name='chessgame',
            name='black',
            field=models.ForeignKey(default='<basic.models.UserSerializer object at 0x7f6587030e20>', on_delete=models.SET('<basic.models.UserSerializer object at 0x7f6587030f10>'), related_name='+', to=settings.AUTH_USER_MODEL),
        ),
        migrations.AlterField(
            model_name='chessgame',
            name='white',
            field=models.ForeignKey(default='<basic.models.UserSerializer object at 0x7f6587030e20>', on_delete=models.SET('<basic.models.UserSerializer object at 0x7f6587030f10>'), related_name='+', to=settings.AUTH_USER_MODEL),
        ),
    ]
