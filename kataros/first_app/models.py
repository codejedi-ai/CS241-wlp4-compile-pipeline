from django.db import models

# Create your models here.
class Topic(models.Model):
    """A topic the user is learning about"""
    top_name = models.CharField(max_length=200)
    date_added = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        """Return a string representation of the model"""
        return self.top_name
class Webpage(models.Model):
    topic = models.ForeignKey(Topic, on_delete=models.CASCADE)
    name = models.CharField(max_length=200)
    url = models.URLField(max_length=200)

    def __str__(self):
        """Return a string representation of the model"""
        return self.name
class AccessRecord(models.Model):
    name = models.ForeignKey(Webpage, on_delete=models.CASCADE)
    date = models.DateField()

    def __str__(self):
        """Return a string representation of the model"""
        return str(self.date)