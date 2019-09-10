#include <stdlib.h>
#include <math.h>                   /* for lrint */
#include "quad.h"
#include "pgmio.h"

/* Reconstruct image from quadtree.  Write into the Image record that im points
 * to.      Assume that the square region described by the quadtree fits into the
 * Image.
 *
 * Recall that each Quad node already contains position information; respect it,
 * and you will benefit from it too (a secret for successful recursions).
 */
/* 
 * Size = 4**depth 
 */
void saveQuad(Image *im, Quad *q)
{   
    if(q->children[0] != NULL) {
		for(int i = 0; i < 4; i++) {
			saveQuad(im, q->children[i]);
		}
	}
	 else {
		for (int i = q->ty; i < q->ty + q->width; i++) {
			for (int j = q->tx; j < q->tx + q->width; j++) {
				im->data[j + i*im->sx] = q->grayscale;
			}
		}
	}
}


/*
 * Computes the maximum difference within a subquadrant of the matrix. 
 */
int maxDiff(Image *im, int tx, int ty, int w){
    int min = 256;
    int max = -1;
    int tmp;
    for(int i = ty;i < ty + w;i++){
        for(int j = tx;j < tx + w;j++){
            tmp = im -> data[j + i * im -> sx];
            if(tmp > max){
                max = tmp;
            }
            if(tmp < min){
                min = tmp;
            }
        }
    }
    return (max - min);
}


/*
 * Sets the Grayscale value for a node accodingly.
 */
void setGrayscale(Quad *root, Image *im, int tx, int ty, int w){
    int gs = 0;
    int size = im -> sx;
    for(int i = ty;i < ty + w; i++){
        for(int j = tx;j < tx + w;j++){
            gs += im -> data[j + i * size];
        }
    }
    root -> grayscale = lrint(gs/(w * w));
}


/* Build quadtree from image.  Splitting stops when maxDepth hits 0 or maximum
 * difference in grayscale is <= threshold.  Do this to only the square region
 * at top-left corner (tx, ty) of width w.  Use heap allocation for all Quad
 * nodes you produce.
 */
Quad *formQuadtree(Image *im, int maxDepth, int threshold, int tx, int ty, int w)
{
    Quad *root = (Quad *) calloc(1, sizeof(Quad));
    root -> tx = tx;
    root -> ty = ty;
    root -> width = w;

    if(maxDepth > 0 && maxDiff(im, tx, ty, w) > threshold){
        root -> children[0] = formQuadtree(im, maxDepth - 1, threshold, tx, ty, w/2);
        root -> children[1] = formQuadtree(im, maxDepth - 1, threshold, tx + w/2, ty, w/2);
        root -> children[2] = formQuadtree(im, maxDepth - 1, threshold, tx, ty + w/2, w/2);
        root -> children[3] = formQuadtree(im, maxDepth - 1, threshold, tx + w/2, ty + w/2, w/2);
    }
    else{
        setGrayscale(root, im, tx, ty, w);
    }
    return root;    
}


/* Deallocate all Quad nodes of the given quadtree. Assume that they all came
 * from the heap.
 */
void freeQuadtree(Quad *q)
{
    if (q == NULL){
        return;
    }
    for(int i = 0; i < 4; i++) {
        freeQuadtree(q->children[i]);
        q->children[i] = NULL;
    }

    free(q);
}