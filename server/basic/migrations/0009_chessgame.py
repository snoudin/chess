# Generated by Django 4.1.7 on 2023-03-13 09:02

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('basic', '0008_user_groups_user_is_superuser_user_user_permissions'),
    ]

    operations = [
        migrations.CreateModel(
            name='ChessGame',
            fields=[
                ('result', models.IntegerField()),
                ('_id', models.IntegerField(primary_key=True, serialize=False)),
            ],
        ),
    ]
