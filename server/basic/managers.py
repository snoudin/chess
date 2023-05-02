from django.contrib.auth.base_user import BaseUserManager
from django.utils.translation import gettext_lazy as _


class CustomUserManager(BaseUserManager):
    """
    Custom user model manager where email is the unique identifiers
    for authentication instead of usernames.
    """
    def create_user(self, email, username, password):
        """
        Create and save a user with the given email and password.
        """
        if not username:
            raise ValueError(_("The username must be set"))
        if not email:
            raise ValueError(_("The Email must be set"))
        email = self.normalize_email(email)
        user = self.model(email=email, username=username)
        user.set_password(password)
        user.save()
        return user

    def create_superuser(self, email, username, password):
        """
        Create and save a SuperUser with the given email and password.
        """
        user = self.create_user(email, username, password)
        user.is_superuser = True
        user.is_active = True
        user.save()
        return user

