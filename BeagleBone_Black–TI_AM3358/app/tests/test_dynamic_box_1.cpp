void test_dynamic_resize()
{
    DynamicBox box;

    for (int day = 1; day <= 28; ++day)
    {
        bool result =
            box.add(
                Date(2026, 1, day),
                "January data");

        assert(result);
    }

    for (int day = 1; day <= 28; ++day)
    {
        bool result =
            box.add(
                Date(2026, 2, day),
                "February data");

        assert(result);
    }

    assert(box.size() == 56);

    std::cout
        << "[PASS] DynamicBox resize\n";
}
