from django.shortcuts import render

# Create your views here.
def index(request):
    my_dict = {'insert_me':"Hello I am the home page!"}
    return render(request,'index.html',context=my_dict)