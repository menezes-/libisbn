from setuptools import setup, find_packages

setup(
        name='pylibisbn',
        version='0.1',
        packages=find_packages(exclude=['tests']),
        url='https://github.com/menezes-/libisbn',
        license='',
        author='Gabriel',
        author_email='',
        description='Simple and Fast ISBN library written in C++',
        test_suite='tests',

)
