#!/usr/bin/python


if __name__ =='__main__':
        import SVMTK as svm
        import argparse
        import os

        parser = argparse.ArgumentParser(description='The remeshing of an input surface using Laplacian or volume preserving Laplacian ')
        parser.add_argument("--i"   ,type=str, help = "The path to the input surface.",required=True)
        parser.add_argument("--iter"   ,type=int  , default=2      ,help = "The number of iterations for the remesh algorithm.")
        parser.add_argument("--edge"   ,type=float, default=1.0    ,help = "The preffered edgelength in the resulting mesh. The default value is 1.0 (mm)")
        parser.add_argument("--protect",action='store_true'  ,help = "Included to protect the edges during remeshing. Note : This option will use more time to finish.")
        parser.add_argument("--o"   ,type=str, help = "The path to the output surface.")
        Z = parser.parse_args() 

        surf  = svm.Surface(Z.i)

        if Z.protect : 
           surf.isotropic_remeshing(Z.edge,Z.iter,True)
        else :
           surf.isotropic_remeshing(Z.edge,Z.iter,False)

        surf.save(Z.o) # make wrtier




