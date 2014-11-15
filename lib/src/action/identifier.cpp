#include "action/identifier.hpp"
#include "id.hpp"
#include "scene.hpp"
#include "state.hpp"
#include "winged/edge.hpp"
#include "winged/face.hpp"
#include "winged/mesh.hpp"
#include "winged/vertex.hpp"
#include "variant.hpp"

struct ActionIdentifier :: Impl {
  Variant <Id, unsigned int> identifier;

  Impl ()                           {}
  Impl (const Id& id)               { this->setId     (id);     }
  Impl (unsigned int index)         { this->setIndex  (index);  }
  Impl (const WingedMesh* mesh)     { this->setMesh   (mesh);   }
  Impl (const WingedFace* face)     { this->setFace   (face);   }
  Impl (const WingedEdge* edge)     { this->setEdge   (edge);   }
  Impl (const WingedVertex* vertex) { this->setVertex (vertex); }
  Impl (const WingedMesh& mesh)     : Impl (&mesh)   {}
  Impl (const WingedFace& face)     : Impl (&face)   {}
  Impl (const WingedEdge& edge)     : Impl (&edge)   {}
  Impl (const WingedVertex& vertex) : Impl (&vertex) {}

  bool hasId () const {
    return this->identifier.is <Id> ();
  }

  bool hasIndex () const {
    return this->identifier.is <unsigned int> ();
  }

  void setId (const Id& id) {
    this->identifier.set (id);
  }

  void setIndex (unsigned int index) {
    this->identifier.set (index);
  }

  void setMesh (const WingedMesh* mesh) {
    if (mesh) {
      this->setId (mesh->id ());
    }
  }

  void setFace (const WingedFace* face) {
    if (face) {
      this->setIndex (face->index ());
    }
  }

  void setEdge (const WingedEdge* edge) {
    if (edge) {
      this->setId (edge->id ());
    }
  }

  void setVertex (const WingedVertex* vertex) {
    if (vertex) {
      this->setIndex (vertex->index ());
    }
  }

  bool isSet () const {
    return this->hasId () || this->hasIndex ();
  }

  Id* getId () const { 
    return this->hasId () ? &this->identifier.get <Id> ()
                          : nullptr;
  }

  unsigned int* getIndex () const {
    return this->hasIndex () ? &this->identifier.get <unsigned int> ()
                             : nullptr;
  }

  WingedMesh* getWingedMesh () const {
    return this->hasId () ? &State::scene ().wingedMesh (*this->getId ()) : nullptr;
  }

  WingedFace* getFace (const WingedMesh& mesh) const {
    return this->hasIndex () ? mesh.face (*this->getIndex ()) : nullptr;
  }

  WingedEdge* getEdge (const WingedMesh& mesh) const {
    return this->hasId () ? mesh.edge (*this->getId ()) : nullptr;
  }

  WingedVertex* getVertex (const WingedMesh& mesh) const {
    return this->hasIndex () ? mesh.vertex (*this->getIndex ()) : nullptr;
  }
};

template <> 
WingedMesh* ActionIdentifier::getMesh <WingedMesh> () const { 
  return this->getWingedMesh (); 
}

DELEGATE_BIG6 (ActionIdentifier)

DELEGATE1_CONSTRUCTOR (ActionIdentifier, const Id&)
DELEGATE1_CONSTRUCTOR (ActionIdentifier, unsigned int)
DELEGATE1_CONSTRUCTOR (ActionIdentifier, const WingedMesh*)
DELEGATE1_CONSTRUCTOR (ActionIdentifier, const WingedFace*)
DELEGATE1_CONSTRUCTOR (ActionIdentifier, const WingedEdge*)
DELEGATE1_CONSTRUCTOR (ActionIdentifier, const WingedVertex*)
DELEGATE1_CONSTRUCTOR (ActionIdentifier, const WingedMesh&)
DELEGATE1_CONSTRUCTOR (ActionIdentifier, const WingedFace&)
DELEGATE1_CONSTRUCTOR (ActionIdentifier, const WingedEdge&)
DELEGATE1_CONSTRUCTOR (ActionIdentifier, const WingedVertex&)
DELEGATE1       (void           , ActionIdentifier, setId, const Id&)
DELEGATE1       (void           , ActionIdentifier, setIndex, unsigned int)
DELEGATE1       (void           , ActionIdentifier, setMesh, const WingedMesh*)
DELEGATE1       (void           , ActionIdentifier, setFace, const WingedFace*)
DELEGATE1       (void           , ActionIdentifier, setEdge, const WingedEdge*)
DELEGATE1       (void           , ActionIdentifier, setVertex, const WingedVertex*)
DELEGATE_CONST  (bool           , ActionIdentifier, isSet)
DELEGATE_CONST  (Id*            , ActionIdentifier, getId)
DELEGATE_CONST  (unsigned int*  , ActionIdentifier, getIndex)
DELEGATE_CONST  (WingedMesh*    , ActionIdentifier, getWingedMesh)
DELEGATE1_CONST (WingedFace*    , ActionIdentifier, getFace, const WingedMesh&)
DELEGATE1_CONST (WingedEdge*    , ActionIdentifier, getEdge, const WingedMesh&)
DELEGATE1_CONST (WingedVertex*  , ActionIdentifier, getVertex, const WingedMesh&)
