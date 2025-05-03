from pathlib import Path

from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext

this_dir = Path(__file__).parent.resolve()

src_files = [
    str(file) for file in (this_dir / "src").glob("*.cpp")
    if file.name != "main.cpp"
]
sources = ["bindings/orderbook_module.cpp"] + src_files

ext_modules = [
    Pybind11Extension(
        'cpp_binance_orderbook',
        sources,
        include_dirs=[
            str(this_dir / 'include'),
            str(this_dir / 'include' / 'enums')
        ],
        language='c++',
    ),
]

setup(
    name='cpp_binance_orderbook',
    version='0.0.3',
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
