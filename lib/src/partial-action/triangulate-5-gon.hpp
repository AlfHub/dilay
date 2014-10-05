#ifndef DILAY_PARTIAL_ACTION_TRIANGULATE_5_GON
#define DILAY_PARTIAL_ACTION_TRIANGULATE_5_GON

#include <unordered_set>
#include "action/on.hpp"
#include "macro.hpp"

class WingedMesh;
class WingedFace;

class PATriangulate5Gon : public ActionOn <WingedMesh> {
  public: 
    DECLARE_BIG3 (PATriangulate5Gon)

  /** `run (m,f,n)` triangulates the 5-gon `f`.
   * Note thate `f.edge ()->firstVertex (f)` must be an even vertex, i.e.
   * it must not be generated during the current subdivision step.
   * The new faces are adjacent to `f`.
   * `f` and the new faces are added to `n` if `n` is not `nullptr`.
   */
    void run (WingedMesh&, WingedFace&, std::unordered_set <WingedFace*>* = nullptr);

  private:
    void runUndo (WingedMesh&);
    void runRedo (WingedMesh&);

    IMPLEMENTATION
};
#endif