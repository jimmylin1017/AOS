/**
 * Optimal Page Replacement.
 *
 * @param frames input the available frames
 * @param numberOfFrames input the number of frames
 * @param references input the sequence of references
 */
private static int optimal(final Frame frames[], final int numberOfFrames, final String... references) {
// Initialize victim
int victim = 0;

// Initialize the number of page fault
int numberOfPageFault = 0;

// Initialize the next reference sequence
final int[] nextSequence = new int[numberOfFrames];
for (int i = 0; i < numberOfFrames; ++i) {
    nextSequence[i] = -1;
}

// Show initial frames information
System.out.printf("Sequence Number: 0%n");
for (int i = 0; i < numberOfFrames; ++i) {
    final Frame frame = frames[i];
    System.out.printf("\tFrame %d%s:%n\t\tReference: %s%n\t\tNext Reference Sequence: %s%n", i + 1, (i == victim) ? "(victim)" : "", frame.reference, (nextSequence[i] == -1) ? "Undefined" : (nextSequence[i] == 0) ? "No" : String.valueOf(nextSequence[i]));
}

// Load references in sequence
final int numberOfReferences = references.length;
for (int n = 0; n < numberOfReferences; ++n) {
    System.out.printf("Sequence Number: %d%n", n + 1);
    final String reference = references[n];
    if (reference == null || reference.equalsIgnoreCase("null")) {
    System.out.printf("\tNothing found to load.%n");
    continue;
    }
    System.out.printf("\tReference: %s%n", reference);

    // Find whether the reference exists in frames or not. If it is not found, the page fault happened.
    boolean fault = true;
    for (int i = 0; i < numberOfFrames; ++i) {
    final Frame frame = frames[i];
    if (reference.equals(frame.reference)) {
        // Break the search loop
        fault = false;
        break;
    }
    }
    System.out.printf("\tPage Fault: %b%n", fault);
    if (fault) {
    // Page fault happened, then use the victim frame to replace its reference.
    frames[victim].reference = reference;
    // Choose next victim
    victim = (victim + 1) % numberOfFrames;

    // Increase the number of page fault
    ++numberOfPageFault;
    }

    // Find the next reference sequence in the future, and optimize the victim
    int tempVictim = victim, tempSequence = nextSequence[victim];
    for (int i = 0; i < numberOfFrames; ++i) {
    final int index = (victim + i) % numberOfFrames;
    final String currentReference = frames[index].reference;
    if (currentReference == null) {
        continue;
    }
    int next;
    for (next = n + 1; next < numberOfReferences; ++next) {
        final String futureReference = references[next];
        if (currentReference.equals(futureReference)) {
        break;
        }
    }
    final int ns;
    if (next == numberOfReferences) {
        ns = 0;
    } else {
        ns = next + 1;
    }
    nextSequence[index] = ns;
    if (tempSequence > 0 && (ns == 0 || ns > tempSequence)) {
        tempSequence = ns;
        tempVictim = index;
    }
    }
    victim = tempVictim;

    for (int i = 0; i < numberOfFrames; ++i) {
    final Frame frame = frames[i];
    System.out.printf("\tFrame %d%s:%n\t\tReference: %s%n\t\tNext Reference Sequence: %s%n", i + 1, (i == victim) ? "(victim)" : "", frame.reference, (nextSequence[i] == -1) ? "Undefined" : (nextSequence[i] == 0) ? "No" : String.valueOf(nextSequence[i]));
    }
    System.out.printf("%n");
}
return numberOfPageFault;
}