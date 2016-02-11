# Element consists of value and link to the next element
class Node(object):

    def __init__(self, set_value, set_node):
        self.value = set_value
        self.next_node = set_node	

    def __str__(self):
        return str(self.value)	


class LinkedList(object):

    def __init__(self):
        self.first = None
        self.length = 0

    def push(self, value):
        new_node = Node(value, self.first)
        self.first = new_node
        self.length += 1

    def pop(self):
        if self.length == 0: return None
        node = self.first
        self.first = node.next_node
        self.length -= 1
        return node

    def edit(self):
        """ Edits list like this 1 2 3 4 to 1 4 2 3. """
        
        original_length = self.length
        first_part_length = original_length - original_length/2

        first_part = LinkedList()
        result_list = LinkedList()

        while first_part.length < first_part_length:
            first_part.push(self.pop())

        if original_length%2 == 1: 
            result_list.push(first_part.pop())

        while first_part.first != None:
            result_list.push(ll.pop())    
            result_list.push(first_part.pop())

        self.first = result_list.first

    def __str__(self):

        output = "LinkedList: "
        current = self.first

        while current != None:
            output += str(current.value) + " "
            current = current.next_node

        return output


if __name__ == '__main__':
    ll = LinkedList()

    for x in range(9):
        ll.push(x)

    print ll
    ll.edit()
    print ll




        
