from rest_framework import serializers
from .models import Patient, HealthProgram, PatientProgram


class HealthProgramSerializer(serializers.ModelSerializer):
    class Meta:
        model = HealthProgram
        fields = ["id", "name", "description"]


class PatientProgramSerializer(serializers.ModelSerializer):
    # program = HealthProgramSerializer(read_only=True)
    program_id = serializers.PrimaryKeyRelatedField(
            queryset=HealthProgram.objects.all(),
            source='program',
            write_only=True)

    class Meta:
        model = PatientProgram
        fields = ['program_id', 'program']


class PatientSerializer(serializers.ModelSerializer):
    programmes = PatientProgramSerializer(many=True)

    class Meta:
        model = Patient
        fields = ["id", "name", "dob", "programmes"]

    def create(self, validated_data):
        programmes_data = validated_data.pop('programmes')
        patient = Patient.objects.create(**validated_data)
        for program_data in programmes_data:
            print("create: program_data: ", program_data)
            PatientProgram.objects.create(patient=patient, **program_data)
        return patient

    def update(self, instance, validated_data):
        programmes_data = validated_data.pop('programmes')
        instance.name = validated_data.get('name', instance.name)
        instance.dob = validated_data.get('dob', instance.dob)
        instance.save()
        print("update", instance,programmes_data )

        # remove and re-add
        instance.programmes.all().delete()
        for program_data in programmes_data:
            print("update: program_data", program_data)
            PatientProgram.objects.create(patient=instance, **program_data)

        return instance
