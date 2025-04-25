# from django.shortcuts import render
from .serializers import HealthProgramSerializer, PatientSerializer
from .models import Patient, HealthProgram
from rest_framework import viewsets


class HealthProgramViewSet(viewsets.ModelViewSet):
    queryset = HealthProgram.objects.all()
    serializer_class = HealthProgramSerializer


class PatientViewSet(viewsets.ModelViewSet):
    queryset = Patient.objects.prefetch_related('programmes__program').all()
    serializer_class = PatientSerializer
