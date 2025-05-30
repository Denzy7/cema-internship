# Generated by Django 5.1.7 on 2025-04-25 10:25

import django.db.models.deletion
from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ("patientmgr", "0005_alter_patientprogram_patient"),
    ]

    operations = [
        migrations.AlterField(
            model_name="patientprogram",
            name="program",
            field=models.ForeignKey(
                blank=True,
                on_delete=django.db.models.deletion.CASCADE,
                to="patientmgr.healthprogram",
            ),
        ),
    ]
