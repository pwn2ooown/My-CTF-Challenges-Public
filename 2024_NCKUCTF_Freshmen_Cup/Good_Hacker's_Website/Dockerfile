# Use the official PHP 8.2 image with Apache
FROM php:8.2-apache

# Enable the mysqli extension for MySQL support
RUN docker-php-ext-install mysqli

# Update package manager and install MySQL client
RUN apt-get update && apt-get install -y default-mysql-client

WORKDIR /var/www/html

# Copy application files into the container
COPY ./app /var/www/html

# Set ownership of uploads and images folders to root and assign group to www-data
RUN chown root:www-data /var/www/html/uploads /var/www/html/images && \
    chmod 775 /var/www/html/uploads /var/www/html/images && \
    chmod +t /var/www/html/uploads /var/www/html/images

# Set permissions for PHP files to be owned by root and readable by all
RUN chown root:root /var/www/html/*.php && \
    chmod 644 /var/www/html/*.php

# Set permissions for specific files in images folder
RUN chown root:root /var/www/html/images/good_hacker.jpg && \
    chmod 644 /var/www/html/images/good_hacker.jpg

# Set permissions for .htaccess in uploads folder
RUN chown root:root /var/www/html/.htaccess && \
    chmod 644 /var/www/html/.htaccess

# Expose port 80 to make the app accessible
EXPOSE 80 