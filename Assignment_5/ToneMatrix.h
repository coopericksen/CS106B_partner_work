/* ToneMatrix.h
 * Cooper Ericksen & Odelia Kislyuk
 * 2026-11-06
 */

#pragma once

#include "Demos/Sample.h"
#include "StringInstrument.h"
#include "GUI/SimpleTest.h"

/* Type that maintains a Tone Matrix, reacts to mouse movement,
 * handles graphics, and sends data to the computer speakers.
 */
class ToneMatrix {
public:
    /* Creates a Tone Matrix whose grid is gridSize x gridSize and
     * where each light in the matrix has size lightSize x lightSize.
     * The lightSize parameter just determines the size, in pixels,
     * of each light in the grid.
     */
    ToneMatrix(int gridSize, int lightSize);

    /* Frees all memory allocated by the ToneMatrix type. */
    ~ToneMatrix();

    /* Reacts to the mouse being pressed at a given location.
     * Specifically, this toggles the state of the light under the
     * mouse.
     */
    void mousePressed(int mouseX, int mouseY);

    /* Reacts to the mouse being dragged at a given location (moved
     * while pressed). Specifically, this updates the light under the
     * mouse to match the state of the light where the mouse was
     * first pressed.
     */
    void mouseDragged(int mouseX, int mouseY);

    /* Draws the Tone Matrix to the screen. */
    void draw() const;

    /* Produces the next sound sample from the Tone Matrix. */
    Sample nextSample();

    /* Resizes the underlying grid of lights. New lights default
     * to being turned off; old lights retain their previous
     * values. Old instruments are preserved. The left-to-right
     * scan of the grid resets back to the leftmost column, as if
     * the Tone Matrix was being created anew.
     */
    void resize(int newGridSize);

private:
    int _gridSize;
    int _lightSize;
    bool* _grid = nullptr;
    StringInstrument* _instruments = nullptr;

    /* Friendly reminder to follow the convention of adding an underscore
     * in front of any private member variables you declare!
     */

    bool _isTurningOn;
    int _calls;
    int _arrow;

    /* Allow SimpleTest test cases to read private fields. */
    ALLOW_TEST_ACCESS();
};
