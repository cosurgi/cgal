// Copyright (c) 2005,2006,2007,2009,2010,2011 Tel-Aviv University (Israel).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you may redistribute it under
// the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL: svn+ssh://balasmic@scm.gforge.inria.fr/svn/cgal/branches/features/Arrangement_on_surface_2-RIC_pl_for_unbounded-balasmic/Arrangement_on_surface_2/include/CGAL/Arr_point_location/Td_active_vertex.h $
// $Id: Td_active_vertex.h 65793 2011-10-10 17:04:38Z balasmic $
// 
//
// Author(s)	 : Oren Nechushtan <theoren@math.tau.ac.il>
//               updated by: Michal Balas <balasmic@post.tau.ac.il>

#ifndef CGAL_TD_ACTIVE_VERTEX_H
#define CGAL_TD_ACTIVE_VERTEX_H

/*! \file
 * Defintion of the Td_active_vertex<Td_traits> class.
 */

#include <CGAL/Arr_point_location/Trapezoidal_decomposition_2.h>
#include <boost/variant.hpp>
#include <boost/shared_ptr.hpp>


#ifdef CGAL_TD_DEBUG
#define CGAL_TD_INLINE
#else
#define CGAL_TD_INLINE inline
#endif

namespace CGAL {

/*! \class
 * Implementation of a pseudo-trapezoid as two halfedges(top,bottom)
 * and two curve-ends(left,right).
 * Trapezoids are represented as two curve-ends called right and left and
 * two halfedges called top and bottom. The curve-ends (points) lie on the 
 * right and left boundaries of the trapezoid respectively and the halfedges 
 * bound the trapezoid from above and below.
 * There exist degenerate trapezoids called infinite trapezoid; this happens 
 * when one of the four sides is on the parameter space boundary.
 * Trapezoids are created as active and become inactive when Remove() member
 * function called.
 * Each trapezoid has at most four neighbouring trapezoids.
 * X_trapezoid structure can represent a real trapezoid, a Td-edge or an 
 * edge-end (end point).
 */
template <class Td_traits_>
class Td_active_vertex : public Handle
{
  public:
  
  //type of traits class
  typedef Td_traits_                                   Traits;
  
  //type of point (Point_2)
  typedef typename Traits::Point                       Point;

  //type of X_monotone_curve_2
  typedef typename Traits::X_monotone_curve_2     X_monotone_curve_2;

  //type of Curve_end
  typedef typename Traits::Curve_end              Curve_end;

  //type of Halfedge_const_handle (trapezoid edge)
  typedef typename Traits::Halfedge_const_handle  Halfedge_const_handle;
  
  //type of Vertex_const_handle (trapezoid vertex)
  typedef typename Traits::Vertex_const_handle    Vertex_const_handle;

  //type of Td_active_vertex (Self)
  typedef typename Traits::Td_active_vertex            Self;
  
  typedef typename Traits::Td_map_item            Td_map_item;

  //type of Trapezoidal decomposition
  typedef Trapezoidal_decomposition_2<Traits>          TD;
  
  //type of In face iterator
  typedef typename TD::In_face_iterator                In_face_iterator;

  //type of Trapezoidal map search structure
  typedef typename TD::Dag_node                 Dag_node;


  //friend class declarations:

  friend class Trapezoidal_decomposition_2<Traits>;
  
#ifdef CGAL_PM_FRIEND_CLASS
#if defined(__SUNPRO_CC) || defined(__PGI) || defined(__INTEL_COMPILER)
  friend class Trapezoidal_decomposition_2<Traits>::In_face_iterator;
#elif defined(__GNUC__)

#if ((__GNUC__ < 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ <= 2)))
  friend typename Trapezoidal_decomposition_2<Traits>::In_face_iterator;
#else
  friend class Trapezoidal_decomposition_2<Traits>::In_face_iterator;
#endif
  
#else
  friend class In_face_iterator;
#endif
#endif
  
   /*! \class
   * Inner class Data derived from Rep class
   */
  class Data : public Rep
  {
    friend class Td_active_vertex<Td_traits_>;

  public:
    //c'tors
    Data (Vertex_const_handle _v,   
          Halfedge_const_handle _bottom_he,
          Halfedge_const_handle _top_he,
          Dag_node* _p_node)
          : v(_v),bottom_he(_bottom_he),top_he(_top_he),p_node(_p_node)
    { }
    
    ~Data() { }

  protected:
    Vertex_const_handle v; 
    Halfedge_const_handle bottom_he;
    Halfedge_const_handle top_he;
    Dag_node* p_node; 
  };
  
 private:
  
  Data* ptr() const { return (Data*)(PTR);  }
	
	
#ifndef CGAL_TD_DEBUG
#ifdef CGAL_PM_FRIEND_CLASS
 protected:
#else
 public: // workaround
#endif
#else //CGAL_TD_DEBUG
 public:
#endif //CGAL_TD_DEBUG
	
//  Dag_node* m_dag_node; //pointer to the search structure (DAG) node
	
  
  /*! Set the DAG node. */
  CGAL_TD_INLINE void set_dag_node(Dag_node* p) 
  {
    ptr()->p_node = p;
  }
  
  /*! Set the trapezoid's left (Vertex_const_handle). */
  inline void set_vertex(Vertex_const_handle v) 
  {
    ptr()->v = v;
  }
  
  /*! Set the trapezoid's bottom (Halfedge_const_handle). */
  inline void set_bottom(Halfedge_const_handle he) 
  {
    if (bottom() != Traits::empty_he_handle() &&
        bottom()->direction() != he->direction())
    {
      ptr()->bottom_he = he->twin();
    }
    else
    {
      ptr()->bottom_he = he;
    }
  }
  
  /*! Set the trapezoid's top (Halfedge_const_handle). */
  inline void set_top(Halfedge_const_handle he) 
  {
    if (top() != Traits::empty_he_handle() &&
        top()->direction() != he->direction())
    {
      ptr()->top_he = he->twin();
    }
    else
    {
      ptr()->top_he = he;
    }
  }

 public:
  
  /// \name Constructors.
  //@{

  Td_active_vertex ()
  {
    PTR = new Data
      (Traits::empty_vtx_handle(), Traits::empty_he_handle(), Traits::empty_he_handle(), NULL);
  }


  /*! Constructor given Vertex & Halfedge handles. */
 Td_active_vertex (Vertex_const_handle v,
                   Halfedge_const_handle btm_he,
                   Halfedge_const_handle top_he,
                   Dag_node* node = 0)
                 
  {
    PTR = new Data (v, btm_he, top_he, node);
  }
   
 
  /*! Copy constructor. */
  Td_active_vertex(const Self& tr) : Handle(tr)
  {
  }
  
  //@}
  
  /// \name Operator overloading.
  //@{

  /*! Assignment operator. 
  *   operator= should not copy m_dag_node (or otherwise update 
  *     Dag_node::replace)
    */
  inline Self& operator= (const Self& t2)
  {
	  Handle::operator=(t2);
	  return *this;
  }

  /*! Operator==. */
  inline bool operator== (const Self& t2) const
  {
    return (ptr() == t2.ptr());
  }

  /*! Operator!=. */
  inline bool operator!= (const Self& t2) const
  {
    return !(operator==(t2));
  }

  //@}


  /// \name Access methods.
  //@{

  inline Self& self() 
  {
    return *this;
  }
  
  inline const Self& self() const 
  {
    return *this;
  }

  /*! Access the trapezoid id (PTR). */
  inline unsigned long id() const
  {
    return (unsigned long) PTR;
  }

  inline Vertex_const_handle vertex() const
  {
    return ptr()->v;
  }

  inline Curve_end curve_end() const
  {
    return Curve_end(vertex()->curve_end());
  }
  
  inline const Point& point() const  
  {
    return vertex()->point();
  }
  
  /*! Access trapezoid bottom. 
  *   filters out the infinite case which returns predefined dummy values
  */
  inline Halfedge_const_handle bottom () const
  {
    return ptr()->bottom_he;
  }
  
  /*! Access trapezoid top. 
  *   filters out the infinite case which returns predefined dummy values
  */
  inline Halfedge_const_handle top () const
  {
    return ptr()->top_he;
  }

  /*! Access DAG node. */
  Dag_node* dag_node() const            {return ptr()->p_node; }
  
  
  //@}
  
  
  

};

} //namespace CGAL

#endif
