from first_app import views
from django.urls import path
from django.urls import re_path
urlpatterns = [
    re_path(r'^$', views.index, name='index'),
]