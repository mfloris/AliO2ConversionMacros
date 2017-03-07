#include <Entities/Track.h>
#include <ecs/Entity.h>
#include <ecs/EntityCollection.h>

using namespace ecs;
using namespace track;
int readNewEvents(const char *file) {
  using Entity_t = Track<ESDEventIndex>;
  Handler h;
  // tell the handler to read all the data from the file we just made.
  // it will only read the first page in memory to read all metadata,
  // afterwards it lazilly loads the components it needs by using mmap
  // (pruning)
  h.readFile(file);

  /// Create an EntityCollection from our handler. This will fetch data from
  /// the handler and fail if components are missing
  EntityCollection<Entity_t> container(h);
  unsigned event_id = 0;
  unsigned event_start = 0;
  std::cout << container.size() << std::endl;
  for (unsigned u = 0; u < container.size(); u++) {
    // container[u] -> the u'th track in the container. this is a Track_t
    // object.
    //
    unsigned this_event = container[u].get<ESDEventIndex>();
    if (this_event != event_id) {
      std::cout << this_event << "\t" << u - event_start << std::endl;
      event_start = u;
      event_id = this_event;
    }
    // std::cout << container[u].get<T>() << "\t";
    // std::cout << container[u].get<X>() << " ";
    // std::cout << container[u].get<Y>() << " ";
    // std::cout << container[u].get<Z>() << std::endl;

    // std::cout << container[u].length_xyz() << std::endl;
  }
  unsigned this_event = container[container.size() - 1].get<ESDEventIndex>();
  std::cout << this_event << "\t" << container.size() - event_start
            << std::endl;
  return 0;
}

int main(int argc, char **argv) { return readNewEvents(argv[1]); }
