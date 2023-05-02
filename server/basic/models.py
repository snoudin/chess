from django.db.migrations.serializer import BaseSerializer
from django.db.migrations.writer import MigrationWriter


from django.db import models
from django.contrib.auth.models import AbstractBaseUser, PermissionsMixin
from django.contrib.auth import get_user_model
from django.utils import timezone
from .managers import CustomUserManager 


class User(AbstractBaseUser, PermissionsMixin):
    email = models.EmailField(primary_key=True)
    username = models.CharField(max_length=100)
    is_active = models.BooleanField(default=True)
    date_joined = models.DateTimeField(default=timezone.now)

    USERNAME_FIELD = "email"
    REQUIRED_FIELDS = ['username']

    objects = CustomUserManager()

    def __str__(self):
        return self.email



class UserSerializer(BaseSerializer):
    def serialize(self):
        return repr(str(self)), {'from basic.models import User'}



MigrationWriter.register_serializer(User, UserSerializer)



class ChessGame(models.Model):
    default_user = get_user_model().objects.get(email='deleted@deleted.com')
    white = models.ForeignKey(User, models.SET(default_user), related_name='+', default=default_user)
    black = models.ForeignKey(User, models.SET(default_user), related_name='+', default=default_user)
    result = models.CharField(max_length=1)
    _id = models.IntegerField(primary_key=True)
    

