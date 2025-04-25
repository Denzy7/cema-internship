from django.contrib import admin
from .models import Patient, HealthProgram, PatientProgram


class PatientProgrammesAdminInline(admin.TabularInline):
    extra = 1
    model = PatientProgram
    autocomplete_fields = ["program"]


@admin.register(HealthProgram)
class HealthProgram(admin.ModelAdmin):
    search_fields = ['name']


@admin.register(Patient)
class PatientAdmin(admin.ModelAdmin):
    list_display = ['name', 'dob']
    inlines = [PatientProgrammesAdminInline]
    search_fields = ['name']


@admin.register(PatientProgram)
class PatientProgramAdmin(admin.ModelAdmin):
    list_display = ['patient', 'program']
    autocomplete_fields = ['patient', 'program']
    search_fields = ['patient__name', 'program__name']

# class ChoiceInline(admin.TabularInline):
    # model = HealthProgram
    # extra = 1


# class QsnAdmin(admin.ModelAdmin):
    # fieldsets = [
        # (None, {"fields": ["qstext"]}),
        # ("Date information", {"fields": ["pubdate"], "classes": ["collapse"]}),
    # ]
    # list_display = ["qstext", "pubdate", "pubrecent"]
    # search_fields = ["qstext"]
    # inlines = [ChoiceInline]
    # list_filter = ["pubdate"]
# # Register your models here.

