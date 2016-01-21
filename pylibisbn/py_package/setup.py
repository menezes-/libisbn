from setuptools import setup, find_packages
import os

def package_data():
    if os.name == 'nt':
        return ["*.pyd", "*.dll"]
    else:
        return ["*.so"]

setup(
        name='pylibisbn',
        version='0.1',
        packages=find_packages(exclude=['tests']),
        url='https://github.com/menezes-/libisbn',
        package_data={"pylibisbn":package_data()},
        license='MIT',
        author='Gabriel',
        author_email='',
        description='Simple and Fast ISBN library written in C++',
        test_suite='tests',
        zip_safe=False

)
