"""Script for converting .asc or .srf to .off or .stl."""
import sys
import os

from argparse import ArgumentParser

from .mesh_conversion_utils import (
    readAsc,
    readOff,
    srf2off,
    srf2off_vec,
    srf2stl,
)


def asc2domain(infilename: str, outfilename: str=None) -> None:
    """Convert asc surface file to stl.

    Arguments:
        infilename: Name of input file
        outfilename: Name of outputfile (optional)

    Surface format will be inferred from `outfilename` if provided, default is off.
    """
    datatuple = readAsc(infilename)

    outpath = outfilename
    if outfilename is None:
        path, extension = os.path.splitext(infilename)
        msg = "Attempting to overwrite file {ifile}. "
        msg += "Specify separate output file".format(infilename)
        assert not infilename.endswith(".off"), msg
        outpath = path + ".off"

    _, out_suffix = os.path.splitext(outpath)
    msg = "Invalid output file format {0}, expects '.off' or '.stl'".format(out_suffix)
    assert out_suffix in {".off", ".stl"}, msg

    if out_suffix == ".off":
        srf2off_vec(datatuple.data, datatuple.num_vertices, datatuple.num_facets, outpath)
    elif out_suffix == ".stl":
        srf2stl(datatuple.data, datatuple.num_vertices, datatuple.num_facets, outpath)


def create_parser() -> ArgumentParser:
    """Create argument parser with option (-i. --input) and optionally (-o, --output)."""
    parser = ArgumentParser(
        description="Convert surface file from .asc to .off or .stl"
    )
    parser.add_argument(
        "-i",
        "--input",
        help="path to input .asc or .srf file",
        required=True
    )
    parser.add_argument(
        "-o",
        "--output",
        help="path to output .off or .stl file. default is input basename as .off"
    )
    return parser


def main() -> None:
    parser = create_parser()
    args = parser.parse_args()
    asc2domain(args.input, args.output)


if __name__ == "__main__":
    main()
