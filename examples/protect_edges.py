import SVMTK as svm


if __name__ == "__main__":
    
 
   surf = svm.Surface();

   surf.make_cylinder(0.,0.,0.,2.,4.,3.,1.0,21)
  
   #surf.split_edges(0.3)
  
   maker = svm.Domain(surf)

   maker.add_sharp_border_edges(surf)


   maker.set_borders()
   maker.default_creating_mesh()
   #maker.refine_mesh()
   maker.save_mesh("cylinder.mesh")
