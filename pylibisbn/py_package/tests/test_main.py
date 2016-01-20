import unittest
from pylibisbn import *


class TestLib(unittest.TestCase):
    def setUp(self):
        self.isbn_map = {"978082-6497529": "0826497527",
                         "978-1-891830-75-4": "18918-30759",
                         "978-1-60309-069-8": "160309069-X",
                         "9780071148160": "0071148167",
                         "9784413008488": "ISBN: 4413008480"}
        self.ugly = ["9781891830754", "978-1-891830-75-4", "isbn 979-10-90636-07-1", "073627099x",
                     "ISBN: 07-362-7099x   ",
                     "0-7362-7004-3"]
        self.pretty = ["9781891830754", "9781891830754", "9791090636071", "073627099X", "073627099X", "0736270043"]

    def test_clean(self):
        """
        Test if clean function correctly remove invalid characters and capitalize appropriate ones
        """

        self.assertSequenceEqual(list(map(clean, self.ugly)), self.pretty)

    def test_check_isbn10(self):
        valids = {
            "073627099x",
            "0736270078",
            "07362-70-043",
            "0736271414",
            "ISBN: 0736270140",
            "4413008480",
            "0234567899"
        }
        invalids = {
            "888183718",
            "9887401392",
            "0-8436-1072-x"
        }
        for i in valids:
            self.assertTrue(is_isbn10(i))
            self.assertTrue(validate(i))

        for i in invalids:
            self.assertFalse(is_isbn10(i))
            self.assertFalse(validate(i))

    def test_check_isbn13(self):
        invalids = {
            "9780826497520",
            "9700000000000",
            "9000000000000",
            "9710000000000",
            "979-1-59940-599-X"
        }
        valids = {
            "978082-6497529",
            "9791090636071",
            "isbn 979-10-90636-07-1",
            "978-1-891830-75-4",
            "978-1-60309-069-8",
            "9780071148160"
        }
        for i in valids:
            self.assertTrue(is_isbn13(i))
            self.assertTrue(validate(i))

        for i in invalids:
            self.assertFalse(is_isbn13(i))
            self.assertFalse(validate(i))

    def test_to_isbn13(self):

        for k, v in self.isbn_map.items():
            self.assertEqual(to_isbn13(v), clean(k))

    def test_to_isbn10(self):
        for k, v in self.isbn_map.items():
            self.assertEqual(to_isbn10(k), clean(v))

    def test_to_isbn13_invalid(self):
        """
        Test if conversion of invalid isbn throws exception
        """
        self.assertRaises(RuntimeError, to_isbn13, "0-8436-1072-x")

    def test_to_isbn10_invalid(self):
        # valid ISBN but prefix 979 cannon be transformed into ISBN10
        self.assertRaises(RuntimeError, to_isbn10, "979-10-90636-07-1")

        self.assertRaises(RuntimeError, to_isbn10, "9780826497520")


if __name__ == '__main__':
    unittest.main()
