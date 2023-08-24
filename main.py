class Solution(object):
    def maximumOr(self, nums, k):
        """
        :type nums: List[int]
        :type k: int
        :rtype: int
        """
        # find the binary length each number in nums
        binary_length = [len(bin(i)) - 2 for i in nums]
        # zip the binary length with the numbers
        binary_length_and_nums = zip(binary_length, nums)
        # sort the list by the binary length
        binary_length_and_nums = sorted(binary_length_and_nums, key=lambda x: x[0])
        # now get the sorted num
        sorted_nums = [i[1] for i in binary_length_and_nums]
        # get prefix or array of the sorted nums
        prefix_or = [sorted_nums[0]]
        for i in range(1, len(sorted_nums)):
            prefix_or.append(prefix_or[-1] | sorted_nums[i])
        # now get the suffix or array of the sorted nums
        suffix_or = [sorted_nums[-1]]
        for i in range(len(sorted_nums) - 2, -1, -1):
            suffix_or.append(suffix_or[-1] | sorted_nums[i])
        suffix_or = suffix_or[::-1]
        # now get the max or
        max_or = 0
        for i in range(len(sorted_nums)):
            max_or = max(max_or, prefix_or[i - 1] | suffix_or[i + 1] | sorted_nums[i])
        return max_or
# write main method
from barttransformer import translate
if __name__ == '__main__':
    translate()
