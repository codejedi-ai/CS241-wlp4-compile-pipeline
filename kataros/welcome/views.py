from django.shortcuts import render

# Create your views here.
def index(request):
    my_dict = {'insert_me':"Hello I am the welcome page!"}
    return render(request,'welcome.html',context=my_dict)