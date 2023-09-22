"""
URL configuration for kataros project.

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/4.2/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.contrib import admin
from django.urls import path
from django.conf.urls import include
# the following is not python installed modules however they are modules that we created
from about_us import views as aboutusviews
from welcome import views as welcomeviews
from home_page import views
urlpatterns = [
    path('', views.index, name='index'),
    path('welcome', welcomeviews.index, name='index'),
    path('about_us', aboutusviews.index, name='index'),

    # include the first_app.urls module
    path('first_app/', include('first_app.urls')),
    # admin page
    path('admin/', admin.site.urls),
]
