from django.shortcuts import render


def online_game(request):
    return render(request, 'play/frontend/game.html')


def bot(request):
    return render(request, 'play/frontend/bot.html')
