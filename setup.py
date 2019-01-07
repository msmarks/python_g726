from distutils.core import setup, Extension

setup(name='python_g726',
      version='1.0',
      description='Python convert between pcm and g726',
      author='mark',
      packages=[],
      ext_modules=[Extension('g726', ['g726cv.c', 'g726lib/g726.c', 'g726lib/g72x.c', 'g726lib/g726_16.c', 'g726lib/g711.c'],
                             include_dirs=['g726lib'],
                             library_dirs=['g726lib']
                             )]
     )