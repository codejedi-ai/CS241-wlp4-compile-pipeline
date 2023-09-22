from django.shortcuts import render
from django.http import HttpResponse

def index(request):
    # return HttpResponse("Hello, world. You're at the about_us index.")
    return render(request,'aboutus.html')
# Create your views here.
