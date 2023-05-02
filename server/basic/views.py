from django.shortcuts import render, redirect
from django.http import HttpResponse
from django.contrib.auth.models import auth
import django.contrib.auth
from .models import User

# Create your views here.

def home(request):
    if request.method == 'POST':
        if request.user.is_authenticated:
            return redirect('/play/infinite/')
        return redirect('/login')
    return render(request, 'basic/frontend/home.html')

def redirect_to_home(request):
    return redirect('/home')

def login(request):
    if request.method == 'POST':
        err_form = 'Wrong email or password'
        email, password = request.POST['email'], request.POST['password']
        cur_user = auth.authenticate(email=email, password=password)
        if cur_user:
            auth.login(request, cur_user)
            return redirect('/home')
        return render(request, 'basic/frontend/login.html', {'form': err_form})
    return render(request, 'basic/frontend/login.html')

def register(request):
    if request.method == 'POST':
        err_form = ''
        email = request.POST['email']
        username = request.POST['username']
        password1 = request.POST['password1']
        password2 = request.POST['password2']
        if not email:
            err_form += 'Email can not be empty\n'
        elif User.objects.filter(email=email).exists():
            err_form += 'Profile with this email already exists\n'
        if not username:
            err_form += 'Username can not be empty\n'
        if not password1:
            err_form += 'Password can not be empty\n'
        if not password2:
            err_form += 'Repeat your password\n'
        if password1 != password2:
            err += 'Password must be same\n'
        if err_form:
            return render(request, 'basic/frontend/registration.html', { 'form': err_form })
        user = User.objects.create_user(email, username, password1)
        user.save()
        return redirect('/home')
    return render(request, 'basic/frontend/registration.html')

def logout(request):
    auth.logout(request)
    return redirect('/home')
