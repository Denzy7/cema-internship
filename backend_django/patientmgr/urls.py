from rest_framework import routers
from debug_toolbar.toolbar import debug_toolbar_urls
from django.urls import path, include
from .views import HealthProgramViewSet, PatientViewSet


router = routers.DefaultRouter()
router.register(r"programmes", HealthProgramViewSet)
router.register(r"patient", PatientViewSet)

app_name = "patientmgr"
urlpatterns = [
    path("api/", include(router.urls)),
    path("api-auth/", include("rest_framework.urls", namespace="rest_framework"))
] + debug_toolbar_urls()
