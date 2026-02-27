/*
The "Rotated Boundary" Challenge
Objective: Given a collection of 2D axis-aligned rectangles, you must:
Calculate the Global Minimum Bounding Box (MBB) that contains every rectangle in the set.

Individually rotate each rectangle 90 degrees around its own geometric center.

Return the total count of rectangles that, after rotation, 
have any part of their area protruding outside the original Global MBB.
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <limits> 
#include <cstdio>

typedef struct {
    float x1; // Lower-left X
    float y1; // Lower-left Y
    float x2; // Upper-right X
    float y2; // Upper-right Y
} Rect;

/**
 * @param rects       Input array of rectangles
 * @param count       Number of rectangles
 * @return            The global MBB encompassing all rects
 */
Rect find_global_mbb(const Rect* rects, size_t count);

/**
 * @param global_mbb  The original boundary
 * @param rects       The rectangles to rotate and check
 * @param count       Number of rectangles
 * @return            The number of rectangles that exceed the global_mbb after 90 deg rotation
 */
int count_out_of_bounds(Rect global_mbb, const Rect* rects, size_t count);

// --- Implementation ---

Rect find_global_mbb(const Rect* rects, size_t count) {
    // TODO: Initialize with extremes and iterate
    return (Rect){0};
}

int count_out_of_bounds(Rect global_mbb, const Rect* rects, size_t count) {
    // Logic Hint: 
    // 1. Find center: cx = (x1 + x2) / 2
    // 2. Current Width = x2 - x1, Current Height = y2 - y1
    // 3. After 90 deg rotation, the new width is the old height (and vice versa)
    // 4. Calculate new x1, y1, x2, y2 based on center and swapped dimensions
    return 0;
}

void print_rect(const char* label, Rect r) {
    printf("%s: LL(%.2f, %.2f) UR(%.2f, %.2f)\n", label, r.x1, r.y1, r.x2, r.y2);
}

// Global failure tracking for CI/CD style reporting
static int total_failures = 0;

void run_test(int num, const char* desc, Rect* rects, size_t count, int expected) {
    Rect mbb = find_global_mbb(rects, count);
    int actual = count_out_of_bounds(mbb, rects, count);
    
    if (actual == expected) {
        printf("[PASS] Test %d: %s\n", num, desc);
    } else {
        printf("[FAIL] Test %d: %s (Expected %d, got %d)\n", num, desc, expected, actual);
        total_failures++;
    }
}

int main() {
    printf("--- Running Geometry Validation Suite ---\n");

    Rect t1[] = { {0, 0, 2, 10} };
    run_test(1, "Self-violation on tall rect", t1, 1, 1);

    Rect t2[] = { {0, 0, 5, 5}, {10, 10, 12, 12} };
    run_test(2, "Perfect squares (invariant)", t2, 2, 0);

    Rect t3[] = { {-10, -5, -8, 5}, {8, -5, 10, 5} };
    run_test(3, "Negative symmetry violations", t3, 2, 2);

    // Final Reporting Logic
    printf("\n---------------------------------------\n");
    if (total_failures == 0) {
        printf("RESULT: ALL TESTS PASSED ✅\n");
    } else {
        printf("RESULT: %d TEST(S) FAILED ❌\n", total_failures);
    }
    printf("---------------------------------------\n");

    return (total_failures == 0) ? 0 : 1; // Return non-zero for script automation
}

/*
Key Interviewer Follow-up Questions:

- Floating Point Comparison: Since coordinates are float, 
  how do you handle cases where a rotated rectangle is "exactly" on the boundary? 
  Would you use an epsilon ($\epsilon$) value?
- Performance: The current approach is O(N). 
  If the rectangles were static but the Global MBB was moving, how would you optimize the search for violations?
- Integer Coordinates: If this were a framebuffer problem using int, how does the "center point" 
  calculation change for rectangles with odd-numbered widths (e.g., width of 5)?
-  Memory Efficiency: If you had a massive dataset, could you perform both calculations (MBB and violation check) in a single pass? 
  (Hint: No, because you need the final MBB to check the first rectangle).
*/