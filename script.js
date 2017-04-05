var quotes = [{"text" : "Difficulties are meant to rouse, not discourage. The human spirit is to grow strong by conflict.",
               "arthor" : "William Ellery Channing."
              },
              
              {"text" : "The most likely way for the world to be destroyed, most experts agree, is by accident. That's where we come in; we're computer professionals. We cause accidents.",
               "arthor" : "Nathaniel Borenstein."
              },
              
              {"text" : "Wisdom is knowing what to do next; virtue is doing it.",
               "arthor" : "David Starr Jordan."
              },
              
              {"text" : "If you wish in this world to advance, your merits you're bound to enhance; You must stir it and stump it, and blow your own trumpet, or trust me, you haven't a chance.",
               "arthor" : "W. S. Gilbert."
              },
              
              {"text" : "A large income is the best recipe for happiness I ever heard of.",
               "arthor" : "Jane Austen."
              },
              
              {"text" : "Behind every great fortune there is a crime.",
               "arthor" : "Honore de Balzac."
              },
              
              {"text" : "The trees that are slow to grow bear the best fruit.",
               "arthor" : "Moliere."
              },
              
              {"text" : "Take hold lightly; let go lightly. This is one of the great secrets of felicity in love.",
               "arthor" : "Spanish Proverb."
              },
              
              {"text" : "It is the final proof of God's omnipotence that he need not exist in order to save us.",
               "arthor" : "Peter De Vries."
              },
              
              {"text" : "Of all afflictions, the worst is self-contempt.",
               "arthor" : "Berthold Auerbach."
              },
              
              {"text" : "I celebrate myself, and sing myself.",
               "arthor" : "Walt Whitman."
              },
              
              {"text" : "Spring is a true reconstructionist.",
               "arthor" : "Henry Timrod."
              },
              
              {"text" : "It's a small world, but I wouldn't want to paint it.",
               "arthor" : "Steven Wright."
              },
              
              {"text" : "A quotation, like a pun, should come unsought, and then be welcomed only for some propriety of felicity justifying the intrusion.",
               "arthor" : "Robert Chapman."
              },
              
              {"text" : "A single day is enough to make us a little larger.",
               "arthor" : "Paul Klee."
              },
              
              {"text" : "The ability to focus attention on important things is a defining characteristic of intelligence.",
               "arthor" : "Robert J. Shiller."
              },
              
               {"text" : "But penance need not be paid in suffering...It can be paid in forward motion. Correcting the mistake is a positive move, a nurturing move.",
               "arthor" : "Barbara Hall."
               },
              
              {"text" : "The best way to convince a fool that he is wrong is to let him have his own way.",
               "arthor" : "Josh Billings."
              },
              
              {"text" : "Silly is you in a natural state, and serious is something you have to do until you can get silly again.",
               "arthor" : "Mike Myers."
              },
              
              {"text" : "The fact that man knows right from wrong proves his intellectual superiority to other creatures; but the fact that he can do wrong proves his moral inferiority to any creature that cannot.",
               "arthor" : "Mark Twain."
              }
              
             ];

var previous = -1;


function randomQuoteNumber() {
    "use strict";
    var num;
    
    do {
        num = Math.random() * quotes.length;
        num = Math.floor(num);
        
    } while (num === previous);
    
    previous = num;
    
    return num;
    
}


