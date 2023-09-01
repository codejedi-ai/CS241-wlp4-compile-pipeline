import help_pages
from help_pages import views
from django.urls import path
from django.urls import re_path
urlpatterns = [
    re_path(r'^$', views.index, name='index'),
]