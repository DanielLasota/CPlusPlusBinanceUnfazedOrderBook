from pathlib import Path

from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext

this_dir = Path(__file__).parent.resolve()

ext_modules = [
    Pybind11Extension(
        'orderbook',
        [
            'bindings/orderbook_module.cpp',
            'src/CSVParser.cpp',
            'src/OrderBook.cpp'
         ],
        include_dirs=[str(this_dir / 'include')],
        language='c++',
    ),
]

setup(
    name='c_plus_plus_binance_unfazed_orderbook',
    version='0.0.1',
    author='Daniel Lasota',
    author_email='grossmann.root@gmail.com',
    description='Orderbook implementation using Pybind11 and C++',
    long_description=open("README.md").read(),
    long_description_content_type="text/markdown",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    url="https://www.youtube.com/watch?v=dQw4w9WgXcQ",
    python_requires=">=3.8",
    install_requires=["pybind11>=2.10.0"],
    include_package_data=True,
)
