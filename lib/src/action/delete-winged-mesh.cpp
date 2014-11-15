#include "action/data.hpp"
#include "action/delete-winged-mesh.hpp"
#include "action/identifier.hpp"
#include "action/unit/on.hpp"
#include "id.hpp"
#include "mesh-type.hpp"
#include "octree.hpp"
#include "partial-action/modify-winged-edge.hpp"
#include "partial-action/modify-winged-face.hpp"
#include "partial-action/modify-winged-mesh.hpp"
#include "partial-action/modify-winged-vertex.hpp"
#include "scene.hpp"
#include "state.hpp"
#include "winged/edge.hpp"
#include "winged/face.hpp"
#include "winged/mesh.hpp"
#include "winged/vertex.hpp"

struct ActionDeleteWMesh::Impl {
  ActionUnitOn <WingedMesh> actions;
  ActionData   <MeshType>   data;

  ActionIdentifier             id;
  MeshType                     meshType;
  std::unique_ptr <WingedMesh> mesh;
  
  void deleteMesh (MeshType t, WingedMesh& mesh) {
    this->data.identifier (mesh);
    this->data.value      (t);

    // reset entities
    mesh.octree ().forEachFace ([this] (WingedFace& f) {
      this->actions.add <PAModifyWFace> ().reset (f);
    });
    for (const WingedVertex& v : mesh.vertices ()) {
      this->actions.add <PAModifyWVertex> ().reset (mesh.vertexRef (v.index ()));
    }
    for (const WingedEdge& e : mesh.edges ()) {
      this->actions.add <PAModifyWEdge> ().reset (mesh.edgeRef (e.id ()));
    }

    // delete entities
    WingedFace* face = nullptr;
    while ((face = mesh.octree ().someFace ()) != nullptr) {
      this->actions.add <PAModifyWMesh> ().deleteFace (mesh, *face);
    }
    while (mesh.numVertices () > 0) {
      WingedVertex& v = mesh.vertexRef (mesh.vertices ().back ().index ());
      this->actions.add <PAModifyWMesh> ().deleteVertex (mesh, v);
    }
    while (mesh.numEdges () > 0) {
      WingedEdge& e = mesh.edgeRef (mesh.edges ().back ().id ());
      this->actions.add <PAModifyWMesh> ().deleteEdge (mesh, e);
    }
    State::scene ().deleteMesh (mesh);
  }

  void runUndo () const {
    WingedMesh& mesh = State::scene ().newWingedMesh ( this->data.value <MeshType> ()
                                                     , this->data.identifier ().getIdRef () );
    this->actions.undo (mesh);
  }
    
  void runRedo () const {
    this->actions.redo (this->data.identifier ().getWingedMeshRef ());
    State::scene ().deleteMesh ( this->data.value <MeshType> ()
                               , this->data.identifier ().getIdRef () );
  }
};

DELEGATE_BIG3   (ActionDeleteWMesh)
DELEGATE2       (void, ActionDeleteWMesh, deleteMesh, MeshType, WingedMesh&)
DELEGATE_CONST  (void, ActionDeleteWMesh, runUndo)
DELEGATE_CONST  (void, ActionDeleteWMesh, runRedo)
