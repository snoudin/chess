from . import views
from django.urls import path
from django.views.generic.base import RedirectView

urlpatterns = [
    path('infinite/', views.online_game),
]
