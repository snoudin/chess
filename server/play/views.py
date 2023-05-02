from django.shortcuts import render

# Create your views here.

def online_game(request):
    return render(request, 'play/frontend/game.html', {'orientation': 'white'})

