namespace Consts {
    const int VIEW_DISTANCE = 2; // radius of the view distance
    const int FULL_VIEW_DISTANCE = VIEW_DISTANCE*2+1; // diameter of the view distance

    const int CHUNK_SIZE = 32; // size of chunk side
    const int CHUNK_SIZE_POW2 = CHUNK_SIZE*CHUNK_SIZE; // 2nd power of chunk side
    const int CHUNK_SIZE_POW3 = CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE; // 3rd power of chunk side
    const int CHUNK_SIZE_HALF = CHUNK_SIZE / 2; // half of the shunk side
    const int CHUNK_LAST_IDX = CHUNK_SIZE - 1; // the last index if we have an array of the length of chunk size

    const unsigned int DEFAULT_SEED = 7;    // The default seed (8th number)
    const unsigned int CHUNK_TOLOAD_BATCH = 9;

    enum BlockIDs {
        air = 0, // first 24 numbers are reserved for transparent blocks
        glass = 1,
        dirt  = 24,
        stone = 25, 
        grass = 26,
    };
}
