from . import views
from django.urls import path

urlpatterns = [
    path('infinite/', views.online_game),
    path('bot/', views.bot),
]
