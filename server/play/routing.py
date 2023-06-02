from django.urls import re_path

from . import consumers

websocket_urlpatterns = [
    re_path(r"ws/play/infinite", consumers.MultiplayerConsumer.as_asgi()),
    re_path(r"ws/play/bot", consumers.BotConsumer.as_asgi()),
]
