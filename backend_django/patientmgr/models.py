from django.db import models


class HealthProgram(models.Model):
    name = models.CharField(max_length=512)
    description = models.CharField(max_length=4096)

    def __str__(self):
        return self.name


class Patient(models.Model):
    name = models.CharField(max_length=512)
    dob = models.DateField("Date of Birth")

    def __str__(self):
        return self.name


class PatientProgram(models.Model):
    patient = models.ForeignKey(Patient, on_delete=models.CASCADE, related_name="programmes")
    program = models.ForeignKey(HealthProgram, on_delete=models.CASCADE)
