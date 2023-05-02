from django.urls import path

from . import views

urlpatterns = [
    path('', views.redirect_to_home, name='home redir'),
    path('home/', views.home, name='home'),
    path('registration/', views.register, name='registration page'),
    path('login/', views.login, name='login page'),
    path('logout/', views.logout, name='logout')

]
